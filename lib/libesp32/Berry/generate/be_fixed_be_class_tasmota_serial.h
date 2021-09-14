#include "be_constobj.h"

static be_define_const_map_slots(be_class_tasmota_serial_map) {
    { be_const_key(SERIAL_5E2, 3), be_const_int(SERIAL_5E2) },
    { be_const_key(SERIAL_8E1, -1), be_const_int(SERIAL_8E1) },
    { be_const_key(SERIAL_8O1, 22), be_const_int(SERIAL_8O1) },
    { be_const_key(SERIAL_5O1, -1), be_const_int(SERIAL_5O1) },
    { be_const_key(read, 0), be_const_func(b_serial_read) },
    { be_const_key(flush, -1), be_const_func(b_serial_flush) },
    { be_const_key(SERIAL_7E2, -1), be_const_int(SERIAL_7E2) },
    { be_const_key(SERIAL_5N2, -1), be_const_int(SERIAL_5N2) },
    { be_const_key(SERIAL_6N2, -1), be_const_int(SERIAL_6N2) },
    { be_const_key(SERIAL_6E2, -1), be_const_int(SERIAL_6E2) },
    { be_const_key(available, -1), be_const_func(b_serial_available) },
    { be_const_key(SERIAL_6N1, 12), be_const_int(SERIAL_6N1) },
    { be_const_key(SERIAL_7O1, -1), be_const_int(SERIAL_7O1) },
    { be_const_key(SERIAL_7N1, 14), be_const_int(SERIAL_7N1) },
    { be_const_key(SERIAL_6O1, -1), be_const_int(SERIAL_6O1) },
    { be_const_key(SERIAL_6E1, -1), be_const_int(SERIAL_6E1) },
    { be_const_key(write, -1), be_const_func(b_serial_write) },
    { be_const_key(SERIAL_8O2, -1), be_const_int(SERIAL_8O2) },
    { be_const_key(SERIAL_5O2, 24), be_const_int(SERIAL_5O2) },
    { be_const_key(SERIAL_5E1, 26), be_const_int(SERIAL_5E1) },
    { be_const_key(deinit, -1), be_const_func(b_serial_deinit) },
    { be_const_key(SERIAL_5N1, -1), be_const_int(SERIAL_5N1) },
    { be_const_key(SERIAL_8N1, 1), be_const_int(SERIAL_8N1) },
    { be_const_key(SERIAL_7E1, -1), be_const_int(SERIAL_7E1) },
    { be_const_key(SERIAL_8N2, -1), be_const_int(SERIAL_8N2) },
    { be_const_key(dot_p, 8), be_const_var(0) },
    { be_const_key(SERIAL_8E2, -1), be_const_int(SERIAL_8E2) },
    { be_const_key(SERIAL_7O2, -1), be_const_int(SERIAL_7O2) },
    { be_const_key(SERIAL_7N2, -1), be_const_int(SERIAL_7N2) },
    { be_const_key(init, -1), be_const_func(b_serial_init) },
    { be_const_key(SERIAL_6O2, 9), be_const_int(SERIAL_6O2) },
};

static be_define_const_map(
    be_class_tasmota_serial_map,
    31
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_tasmota_serial,
    1,
    NULL,
    serial
);
