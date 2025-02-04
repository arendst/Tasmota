#include "be_constobj.h"

static be_define_const_map_slots(be_class_range_map) {
    { be_const_key(__lower__, 8), be_const_var(0) },
    { be_const_key(__incr__, -1), be_const_var(2) },
    { be_const_key(setrange, -1), be_const_func(m_setrange) },
    { be_const_key(__upper__, -1), be_const_var(1) },
    { be_const_key(init, -1), be_const_func(m_init) },
    { be_const_key(tostring, 4), be_const_func(m_tostring) },
    { be_const_key(incr, -1), be_const_func(m_incr) },
    { be_const_key(upper, 6), be_const_func(m_upper) },
    { be_const_key(lower, -1), be_const_func(m_lower) },
    { be_const_key(iter, -1), be_const_func(m_iter) },
};

static be_define_const_map(
    be_class_range_map,
    10
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_range,
    3,
    NULL,
    range
);
