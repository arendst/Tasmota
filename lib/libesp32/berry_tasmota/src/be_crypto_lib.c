/********************************************************************
 * Berry module `webserver`
 * 
 * To use: `import webserver`
 * 
 * Allows to respond to HTTP request
 *******************************************************************/
#include "be_constobj.h"
#include "be_mapping.h"

extern int be_class_crypto_member(bvm *vm);
extern int m_crypto_random(bvm *vm);

extern int m_rsa_rsassa_pkcs1_v1_5(bvm *vm);

extern int m_aes_ccm_init(bvm *vm);
extern int m_aes_ccm_encrypt(bvm *vm);
extern int m_aes_ccm_encrypt1(bvm *vm);
extern int m_aes_ccm_decrypt(bvm *vm);
extern int m_aes_ccm_decrypt1(bvm *vm);
extern int m_aes_ccm_tag(bvm *vm);

extern int m_aes_gcm_init(bvm *vm);
extern int m_aes_gcm_encryt(bvm *vm);
extern int m_aes_gcm_decryt(bvm *vm);
extern int m_aes_gcm_tag(bvm *vm);

extern int m_aes_ctr_init(bvm *vm);
extern int m_aes_ctr_run(bvm *vm);
extern int m_aes_ctr_tag(bvm *vm);

extern int m_ec_p256_pubkey(bvm *vm);
extern int m_ec_p256_sharedkey(bvm *vm);
extern int m_ec_p256_ecdsa_sign_sha256(bvm *vm);
extern int m_ec_p256_ecdsa_verify_sha256(bvm *vm);
extern int m_ec_p256_ecdsa_sign_sha256_asn1(bvm *vm);
extern int m_ec_p256_ecdsa_verify_sha256_asn1(bvm *vm);
extern int m_ec_p256_mod(bvm *vm);
extern int m_ec_p256_neg(bvm *vm);
extern int m_ec_p256_muladd(bvm *vm);
extern int m_ec_p256_mul(bvm *vm);

extern int m_ec_c25519_pubkey(bvm *vm);
extern int m_ec_c25519_sharedkey(bvm *vm);

extern int m_hash_sha256_init(bvm *vm);
extern int m_hash_sha256_update(bvm *vm);
extern int m_hash_sha256_out(bvm *vm);

extern int m_hmac_sha256_init(bvm *vm);
extern int m_hmac_sha256_update(bvm *vm);
extern int m_hmac_sha256_out(bvm *vm);

extern int m_pbkdf2_hmac_sha256_f(bvm *vm);

extern int m_hkdf_sha256_derive(bvm *vm);

extern const bclass be_class_md5;

#include "solidify/solidified_crypto_pbkdf2_hmac_sha256.h"
#include "solidify/solidified_crypto_spake2p_matter.h"

#include "be_fixed_be_class_rsa.h"
#include "be_fixed_be_class_aes_ccm.h"
#include "be_fixed_be_class_aes_gcm.h"
#include "be_fixed_be_class_aes_ctr.h"
#include "be_fixed_be_class_ec_p256.h"
#include "be_fixed_be_class_ec_c25519.h"
#include "be_fixed_be_class_sha256.h"
#include "be_fixed_be_class_hmac_sha256.h"
#include "be_fixed_be_class_pbkdf2_hmac_sha256.h"
#include "be_fixed_be_class_hkdf_sha256.h"
#include "be_fixed_crypto.h"

const be_const_member_t be_crypto_members[] = {
  // name with prefix '/' indicates a Berry class
  // entries need to be sorted (ignoring the prefix char)
#ifdef USE_BERRY_CRYPTO_AES_CCM
  { "/AES_CCM", (intptr_t) &be_class_aes_ccm },
#endif // USE_BERRY_CRYPTO_AES_CTR

#ifdef USE_BERRY_CRYPTO_AES_CTR
  { "/AES_CTR", (intptr_t) &be_class_aes_ctr },
#endif // USE_BERRY_CRYPTO_AES_CTR

#ifdef USE_BERRY_CRYPTO_AES_GCM
  { "/AES_GCM", (intptr_t) &be_class_aes_gcm },
#endif // USE_BERRY_CRYPTO_AES_GCM

#ifdef USE_BERRY_CRYPTO_EC_C25519
  { "/EC_C25519", (intptr_t) &be_class_ec_c25519 },
#endif // USE_BERRY_CRYPTO_EC_C25519

#ifdef USE_BERRY_CRYPTO_EC_P256
  { "/EC_P256", (intptr_t) &be_class_ec_p256 },
#endif // USE_BERRY_CRYPTO_EC_P256

#ifdef USE_BERRY_CRYPTO_HKDF_SHA256
  { "/HKDF_SHA256", (intptr_t) &be_class_hkdf_sha256 },
#endif // USE_BERRY_CRYPTO_HKDF_SHA256

#ifdef USE_BERRY_CRYPTO_HMAC_SHA256
  { "/HMAC_SHA256", (intptr_t) &be_class_hmac_sha256 },
#endif // USE_BERRY_CRYPTO_HMAC_SHA256

  { "/MD5", (intptr_t) &be_class_md5 },

#ifdef USE_BERRY_CRYPTO_PBKDF2_HMAC_SHA256
  { "/PBKDF2_HMAC_SHA256", (intptr_t) &be_class_pbkdf2_hmac_sha256 },
#endif // USE_BERRY_CRYPTO_PBKDF2_HMAC_SHA256

#ifdef USE_BERRY_CRYPTO_RSA
  { "/RSA", (intptr_t) &be_class_rsa },
#endif // USE_BERRY_CRYPTO_PBKDF2_HMAC_SHA256

#ifdef USE_BERRY_CRYPTO_SHA256
  { "/SHA256", (intptr_t) &be_class_sha256 },
#endif // USE_BERRY_CRYPTO_SHA256

#ifdef USE_BERRY_CRYPTO_SPAKE2P_MATTER
  { "/SPAKE2P_Matter", (intptr_t) &be_class_SPAKE2P_Matter },
#endif // USE_BERRY_CRYPTO_SPAKE2P_MATTER

};
const size_t be_crypto_members_size = sizeof(be_crypto_members)/sizeof(be_crypto_members[0]);


/* @const_object_info_begin

class be_class_rsa (scope: global, name: RSA) {
    rs256, static_func(m_rsa_rsassa_pkcs1_v1_5)
}

class be_class_aes_ccm (scope: global, name: AES_CCM) {
    .p1, var
    .p2, var

    init, func(m_aes_ccm_init)
    encrypt, func(m_aes_ccm_encrypt)
    decrypt, func(m_aes_ccm_decrypt)
    tag, func(m_aes_ccm_tag)

    decrypt1, static_func(m_aes_ccm_decrypt1)
    encrypt1, static_func(m_aes_ccm_encrypt1)
}

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

class be_class_ec_p256 (scope: global, name: EC_P256) {
    public_key, static_func(m_ec_p256_pubkey)
    shared_key, static_func(m_ec_p256_sharedkey)
    ecdsa_sign_sha256, static_func(m_ec_p256_ecdsa_sign_sha256)
    ecdsa_verify_sha256, static_func(m_ec_p256_ecdsa_verify_sha256)
    ecdsa_sign_sha256_asn1, static_func(m_ec_p256_ecdsa_sign_sha256_asn1)
    ecdsa_verify_sha256_asn1, static_func(m_ec_p256_ecdsa_verify_sha256_asn1)
    mod, static_func(m_ec_p256_mod)
    neg, static_func(m_ec_p256_neg)
    muladd, static_func(m_ec_p256_muladd)
    mul, static_func(m_ec_p256_mul)
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

class be_class_pbkdf2_hmac_sha256 (scope: global, name: PBKDF2_HMAC_SHA256) {
    _f, static_func(m_pbkdf2_hmac_sha256_f)
    //_f, static_closure(_f_closure)        // this is the slow Berry reference version
    derive, closure(PBKDF2_HMAC_SHA256_closure)
}

class be_class_hkdf_sha256 (scope: global, name: HKDF_SHA256) {
    derive, static_func(m_hkdf_sha256_derive)
}

module crypto (scope: global) {
  member, func(be_class_crypto_member)
  random, func(m_crypto_random)
}

@const_object_info_end */
