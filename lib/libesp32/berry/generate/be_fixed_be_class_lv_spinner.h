#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_spinner_map) {
    { be_const_key_weak(_arc_anim_end_angle, -1), be_const_comptr(&arc_anim_end_angle) },
    { be_const_key_weak(_arc_anim_start_angle, -1), be_const_comptr(&arc_anim_start_angle) },
    { be_const_key_weak(_class, -1), be_const_comptr(&lv_spinner_class) },
    { be_const_key_weak(init, -1), be_const_func(be_ntv_lv_spinner_init) },
};

static be_define_const_map(
    be_class_lv_spinner_map,
    4
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_spinner,
    0,
    (bclass *)&be_class_lv_arc,
    lv_spinner
);
