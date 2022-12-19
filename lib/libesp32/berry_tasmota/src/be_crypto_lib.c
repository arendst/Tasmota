/********************************************************************
 * Berry module `webserver`
 * 
 * To use: `import webserver`
 * 
 * Allows to respond to HTTP request
 *******************************************************************/
#include "be_constobj.h"

extern int m_aes_gcm_init(bvm *vm);
extern int m_aes_gcm_encryt(bvm *vm);
extern int m_aes_gcm_decryt(bvm *vm);
extern int m_aes_gcm_tag(bvm *vm);

extern int m_aes_ctr_init(bvm *vm);
extern int m_aes_ctr_run(bvm *vm);
extern int m_aes_ctr_tag(bvm *vm);

extern int m_ec_c25519_pubkey(bvm *vm);
extern int m_ec_c25519_sharedkey(bvm *vm);

extern int m_hash_sha256_init(bvm *vm);
extern int m_hash_sha256_update(bvm *vm);
extern int m_hash_sha256_out(bvm *vm);

extern int m_hmac_sha256_init(bvm *vm);
extern int m_hmac_sha256_update(bvm *vm);
extern int m_hmac_sha256_out(bvm *vm);

extern const bclass be_class_md5;

#include "be_fixed_be_class_aes_gcm.h"
#include "be_fixed_be_class_aes_ctr.h"
#include "be_fixed_be_class_ec_c25519.h"
#include "be_fixed_be_class_sha256.h"
#include "be_fixed_be_class_hmac_sha256.h"
#include "be_fixed_crypto.h"

/* @const_object_info_begin

class be_class_aes_gcm (scope: global, name: AES_GCM) {
    .p1, var
    .p2, var

    init, func(m_aes_gcm_init)
    encrypt, func(m_aes_gcm_encryt)
    decrypt, func(m_aes_gcm_decryt)
    tag, func(m_aes_gcm_tag)
}

class be_class_aes_ctr (scope: global, name: AES_CTR) {
    .p1, var

    init, func(m_aes_ctr_init)
    encrypt, func(m_aes_ctr_run)
    decrypt, func(m_aes_ctr_run)
}

class be_class_ec_c25519 (scope: global, name: EC_C25519) {
    public_key, func(m_ec_c25519_pubkey)
    shared_key, func(m_ec_c25519_sharedkey)
}

class be_class_sha256 (scope: global, name: SHA256) {
    .p, var

    init, func(m_hash_sha256_init)
    update, func(m_hash_sha256_update)
    out, func(m_hash_sha256_out)
}

class be_class_hmac_sha256 (scope: global, name: HMAC_SHA256) {
    .p, var

    init, func(m_hmac_sha256_init)
    update, func(m_hmac_sha256_update)
    out, func(m_hmac_sha256_out)
}

module crypto (scope: global) {
  AES_GCM, class(be_class_aes_gcm)
  AES_CTR, class(be_class_aes_ctr)
  EC_C25519, class(be_class_ec_c25519)
  SHA256, class(be_class_sha256)
  HMAC_SHA256, class(be_class_hmac_sha256)
  MD5, class(be_class_md5)
}

@const_object_info_end */
