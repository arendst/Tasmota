#include "be_constobj.h"

static be_define_const_map_slots(m_libintrospect_map) {
    { be_const_key(members, -1), be_const_func(m_attrlist) },
    { be_const_key(vcall, -1), be_const_func(m_vcall) },
    { be_const_key(get, -1), be_const_func(m_findmember) },
    { be_const_key(set, 2), be_const_func(m_setmember) },
};

static be_define_const_map(
    m_libintrospect_map,
    4
);

static be_define_const_module(
    m_libintrospect,
    "introspect"
);

BE_EXPORT_VARIABLE be_define_const_native_module(introspect, NULL);
