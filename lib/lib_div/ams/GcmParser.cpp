
#include "GcmParser.h"
#include "tasmota_options.h"

#ifdef USE_TLS
#include "lwip/def.h"
#include <t_bearssl.h>

GCMParser::GCMParser(uint8_t *encryption_key, uint8_t *authentication_key) {
    memcpy(this->encryption_key, encryption_key, 16);
    memcpy(this->authentication_key, authentication_key, 16);
    use_auth = 0;
    for (uint16_t cnt = 0; cnt < 16; cnt++) {
      if (authentication_key[cnt]) {
        use_auth |= 1;
      }
    }
}

int8_t GCMParser::parse(uint8_t *d, DataParserContext &ctx) {
    if(ctx.length < 12) return DATA_PARSE_INCOMPLETE;

    uint8_t* ptr = (uint8_t*) d;
    if(*ptr != GCM_TAG) return DATA_PARSE_BOUNDRY_FLAG_MISSING;
    ptr++;
    // Encrypted APDU
    // http://www.weigu.lu/tutorials/sensors2bus/04_encryption/index.html

    uint8_t systemTitleLength = *ptr;
    ptr++;

    uint8_t initialization_vector[12];
    memcpy(ctx.system_title, ptr, systemTitleLength);
    memcpy(initialization_vector, ctx.system_title, systemTitleLength);

    int len = 0;
    int headersize = 2 + systemTitleLength;
    ptr += systemTitleLength;

    if (ctx.flags & 1) {
      len = *ptr;
        ptr++;
        headersize++;
    } else {
      if(((*ptr) & 0xFF) == 0x81) {
        ptr++;
        len = *ptr;
        // 1-byte payload length
        ptr++;
        headersize += 2;
      } else if(((*ptr) & 0xFF) == 0x82) {
        GCMSizeDef* h = (GCMSizeDef*) ptr;

        // 2-byte payload length
        len = (ntohs(h->format) & 0xFFFF);

        ptr += 3;
        headersize += 3;
      } else  if(((*ptr) & 0xFF) == 0x4f) {
        // ???????? single frame did only decode with this compare
        ptr++;
        headersize++;
      } else  if(((*ptr) & 0xFF) == 0x5e) {
        // ???????? single frame did only decode with this compare
        ptr++;
        headersize++;
      } else {
        len = *ptr;
        ptr++;
        headersize++;
      }
    }
    if(len + headersize > ctx.length)
        return DATA_PARSE_INCOMPLETE;

    uint8_t additional_authenticated_data[17];
    memcpy(additional_authenticated_data, ptr, 1);

    // Security tag
    uint8_t sec = *ptr;
    ptr++;
    headersize++;

    // Frame counter
    memcpy(initialization_vector + 8, ptr, 4);
    ptr += 4;
    headersize += 4;

    int footersize = 0;

    // Authentication enabled
    uint8_t authentication_tag[12];
    uint8_t authkeylen = 0, aadlen = 0;
    if((sec & 0x10) == 0x10) {
        authkeylen = 12;
        aadlen = 17;
        footersize += authkeylen;
        memcpy(additional_authenticated_data + 1, authentication_key, 16);
        memcpy(authentication_tag, ptr + len - footersize - 5, authkeylen);
    }

    br_gcm_context gcm_ctx;
  	br_aes_small_ctr_keys ctr_ctx;
  	br_aes_small_ctr_init(&ctr_ctx, encryption_key, 16);
  	br_gcm_init(&gcm_ctx, &ctr_ctx.vtable, &br_ghash_ctmul32);
    br_gcm_reset(&gcm_ctx, initialization_vector, 12);
    if (use_auth && authkeylen > 0) {
      br_gcm_aad_inject(&gcm_ctx, additional_authenticated_data, aadlen);
    }
    br_gcm_flip(&gcm_ctx);
  	br_gcm_run(&gcm_ctx, 0, ptr , ctx.length - headersize);
    if (use_auth && authkeylen > 0 && br_gcm_check_tag_trunc(&gcm_ctx, authentication_tag, authkeylen) != 1) {
      return GCM_AUTH_FAILED;
    }

    ctx.length -= footersize + headersize;
    return ptr - d;
}
#endif // USE_TLS
