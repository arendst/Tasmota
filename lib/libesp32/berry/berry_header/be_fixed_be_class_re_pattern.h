#include "be_constobj.h"

static be_define_const_map_slots(be_class_re_pattern_map) {
    { be_const_key(searchall, 4), be_const_func(re_pattern_search_all) },
    { be_const_key(matchall, -1), be_const_func(re_pattern_match_all) },
    { be_const_key(match, -1), be_const_func(re_pattern_match) },
    { be_const_key(match2, 2), be_const_func(re_pattern_match2) },
    { be_const_key(split, -1), be_const_func(re_pattern_split) },
    { be_const_key(_p, -1), be_const_var(0) },
    { be_const_key(search, 5), be_const_func(re_pattern_search) },
};

static be_define_const_map(
    be_class_re_pattern_map,
    7
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_re_pattern,
    1,
    NULL,
    re_pattern
);
