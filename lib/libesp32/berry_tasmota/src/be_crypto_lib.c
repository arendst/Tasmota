/********************************************************************
 * Berry module `webserver`
 * 
 * To use: `import webserver`
 * 
 * Allows to respond to HTTP request
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_ALEXA_AVS

extern int m_aes_gcm_init(bvm *vm);
extern int m_aes_gcm_encryt(bvm *vm);
extern int m_aes_gcm_decryt(bvm *vm);
extern int m_aes_gcm_tag(bvm *vm);

extern int m_ec_c25519_pubkey(bvm *vm);
extern int m_ec_c25519_sharedkey(bvm *vm);

#include "be_fixed_be_class_aes_gcm.h"
#include "be_fixed_be_class_ec_c25519.h"
#include "be_fixed_crypto.h"

/* @const_object_info_begin

class be_class_aes_gcm (scope: global, name: AES_GCM, strings: weak) {
    .p1, var
    .p2, var

    init, func(m_aes_gcm_init)
    encrypt, func(m_aes_gcm_encryt)
    decrypt, func(m_aes_gcm_decryt)
    tag, func(m_aes_gcm_tag)
}

class be_class_ec_c25519 (scope: global, name: EC_C25519, strings: weak) {
    public_key, func(m_ec_c25519_pubkey)
    shared_key, func(m_ec_c25519_sharedkey)
}

module crypto (scope: global, strings: weak) {
  AES_GCM, class(be_class_aes_gcm)
  EC_C25519, class(be_class_ec_c25519)
}

@const_object_info_end */

#endif // USE_ALEXA_AVS
