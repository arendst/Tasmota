#include "be_constobj.h"

static be_define_const_map_slots(m_libunishox_map) {
    { be_const_key(decompress, -1), be_const_func(be_ntv_unishox_decompress) },
    { be_const_key(compress, -1), be_const_func(be_ntv_unishox_compress) },
};

static be_define_const_map(
    m_libunishox_map,
    2
);

static be_define_const_module(
    m_libunishox,
    "unishox"
);

BE_EXPORT_VARIABLE be_define_const_native_module(unishox);
