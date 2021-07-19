#include "be_constobj.h"

static be_define_const_map_slots(be_class_md5_map) {
    { be_const_key(update, -1), be_const_func(m_md5_update) },
    { be_const_key(dot_p, -1), be_const_var(0) },
    { be_const_key(finish, -1), be_const_func(m_md5_finish) },
    { be_const_key(init, 1), be_const_func(m_md5_init) },
};

static be_define_const_map(
    be_class_md5_map,
    4
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_md5,
    1,
    NULL,
    MD5
);
