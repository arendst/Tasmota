#include "be_constobj.h"

static be_define_const_map_slots(m_libintrospect_map) {
    { be_const_key(members, 3), be_const_func(m_attrlist) },
    { be_const_key(set, -1), be_const_func(m_setmember) },
    { be_const_key(get, -1), be_const_func(m_findmember) },
    { be_const_key(toptr, -1), be_const_func(m_toptr) },
    { be_const_key(ismethod, -1), be_const_func(m_ismethod) },
    { be_const_key(fromptr, 2), be_const_func(m_fromptr) },
};

static be_define_const_map(
    m_libintrospect_map,
    6
);

static be_define_const_module(
    m_libintrospect,
    "introspect"
);

BE_EXPORT_VARIABLE be_define_const_native_module(introspect);
