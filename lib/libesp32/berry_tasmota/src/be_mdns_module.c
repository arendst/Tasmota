/********************************************************************
 * Berry module `mdns`
 * 
 * To use: `import mdns`
 * 
 * MDNS support
 *******************************************************************/
#include "be_constobj.h"
#include "be_mapping.h"

#ifdef USE_DISCOVERY
#include "mdns.h"

static int32_t m_mdns_init(struct bvm *vm) {
  esp_err_t err = mdns_init();
  if (err != ESP_OK) {
    be_raisef(vm, "internal_error", "could not initialize mdns err=%i", err);
  }
  be_pushvalue(vm, 1);    // push the value of first arg which is the module itself
  be_return(vm);
}

static void m_mdns_set_hostname(struct bvm *vm, const char * hostname) {
  esp_err_t err = mdns_hostname_set(hostname);
  if (err != ESP_OK) {
    be_raisef(vm, "internal_error", "mdns set_hostname err=%i", err);
  }
}
BE_FUNC_CTYPE_DECLARE(m_mdns_set_hostname, "", "@s")

// static uint32_t c_crc16(uint32_t crc, const uint8_t* buf, size_t size) {
//   return crc16_le(crc, buf, size);
// }
// BE_FUNC_CTYPE_DECLARE(c_crc16, "i", "i(bytes)~")

// static uint32_t c_crc8(uint32_t crc, const uint8_t* buf, size_t size) {
//   return crc8_le(crc, buf, size);
// }
// BE_FUNC_CTYPE_DECLARE(c_crc8, "i", "i(bytes)~")

// static uint32_t c_sum(const uint8_t* buf, size_t size) {
//   uint32_t sum = 0;
//   for (uint32_t i = 0; i < size; i++) {
//     sum = (sum + buf[i]) & 0xFF;
//   }
//   return sum;
// }
// BE_FUNC_CTYPE_DECLARE(c_sum, "i", "(bytes)~")

/* @const_object_info_begin
module mdns (scope: global) {
    init, func(m_mdns_init)
    set_hostname, ctype_func(m_mdns_set_hostname)
//   crc32, ctype_func(c_crc32)
//   crc16, ctype_func(c_crc16)
//   crc8, ctype_func(c_crc8)
//   sum, ctype_func(c_sum)
}
@const_object_info_end */
#include "be_fixed_mdns.h"

#endif // USE_DISCOVERY