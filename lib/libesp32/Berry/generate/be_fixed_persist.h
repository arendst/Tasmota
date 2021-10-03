#include "be_constobj.h"

static be_define_const_map_slots(m_libpersist_map) {
    { be_const_key(init, -1), be_const_closure(_anonymous__closure) },
};

static be_define_const_map(
    m_libpersist_map,
    1
);

static be_define_const_module(
    m_libpersist,
    "persist"
);

BE_EXPORT_VARIABLE be_define_const_native_module(persist, NULL);
