#include "be_constobj.h"

static be_define_const_map_slots(be_class_tasmota_log_reader_map) {
    { be_const_key(get_log, 1), be_const_ctype_func(tlr_get_log) },
    { be_const_key(init, -1), be_const_ctype_func(tlr_init) },
    { be_const_key(_p, -1), be_const_var(0) },
};

static be_define_const_map(
    be_class_tasmota_log_reader_map,
    3
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_tasmota_log_reader,
    1,
    NULL,
    tasmota_log_reader
);
