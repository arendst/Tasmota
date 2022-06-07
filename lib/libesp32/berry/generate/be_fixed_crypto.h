#include "be_constobj.h"

static be_define_const_map_slots(m_libcrypto_map) {
    { be_const_key_weak(AES_GCM, -1), be_const_class(be_class_aes_gcm) },
    { be_const_key_weak(EC_C25519, -1), be_const_class(be_class_ec_c25519) },
};

static be_define_const_map(
    m_libcrypto_map,
    2
);

static be_define_const_module(
    m_libcrypto,
    "crypto"
);

BE_EXPORT_VARIABLE be_define_const_native_module(crypto);
