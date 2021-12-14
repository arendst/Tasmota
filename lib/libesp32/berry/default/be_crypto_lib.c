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

#include "../generate/be_fixed_be_class_aes_gcm.h"
#include "../generate/be_fixed_be_class_ec_c25519.h"

void be_load_crypto_lib(bvm *vm) {
    // insert the class GCM in module AES
    be_newmodule(vm);
    be_setname(vm, -1, "crypto");
    be_setglobal(vm, "crypto");

    be_pushntvclass(vm, &be_class_aes_gcm);
    be_setmember(vm, -2, "AES_GCM");
    be_pop(vm, 1);

    be_pushntvclass(vm, &be_class_ec_c25519);
    be_setmember(vm, -2, "EC_C25519");
    be_pop(vm, 2);
}
/* @const_object_info_begin

class be_class_aes_gcm (scope: global, name: AES_GCM) {
    .p1, var
    .p2, var

    init, func(m_aes_gcm_init)
    encrypt, func(m_aes_gcm_encryt)
    decrypt, func(m_aes_gcm_decryt)
    tag, func(m_aes_gcm_tag)
}

class be_class_ec_c25519 (scope: global, name: EC_C25519) {
    public_key, func(m_ec_c25519_pubkey)
    shared_key, func(m_ec_c25519_sharedkey)
}

@const_object_info_end */

#endif // USE_ALEXA_AVS
