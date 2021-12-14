#include "be_constobj.h"

static be_define_const_map_slots(be_class_ec_c25519_map) {
    { be_const_key(public_key, 1), be_const_func(m_ec_c25519_pubkey) },
    { be_const_key(shared_key, -1), be_const_func(m_ec_c25519_sharedkey) },
};

static be_define_const_map(
    be_class_ec_c25519_map,
    2
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_ec_c25519,
    0,
    NULL,
    EC_C25519
);
