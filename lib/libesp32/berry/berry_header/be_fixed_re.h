#include "be_constobj.h"

static be_define_const_map_slots(m_libre_map) {
    { be_const_key(searchall, 5), be_const_func(be_re_search_all) },
    { be_const_key(matchall, -1), be_const_func(be_re_match_all) },
    { be_const_key(match, -1), be_const_func(be_re_match) },
    { be_const_key(match2, 2), be_const_func(be_re_match2) },
    { be_const_key(compile, -1), be_const_func(be_re_compile) },
    { be_const_key(split, -1), be_const_func(be_re_split) },
    { be_const_key(search, 4), be_const_func(be_re_search) },
};

static be_define_const_map(
    m_libre_map,
    7
);

static be_define_const_module(
    m_libre,
    "re"
);

BE_EXPORT_VARIABLE be_define_const_native_module(re);
