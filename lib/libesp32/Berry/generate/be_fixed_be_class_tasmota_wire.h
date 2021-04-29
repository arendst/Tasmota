#include "be_constobj.h"

static be_define_const_map_slots(be_class_tasmota_wire_map) {
    { be_const_key(_end_transmission, 10), be_const_func(b_wire_endtransmission) },
    { be_const_key(bus, -1), be_const_int(0) },
    { be_const_key(_read, 7), be_const_func(b_wire_read) },
    { be_const_key(read, -1), be_const_func(b_wire_validread) },
    { be_const_key(_request_from, -1), be_const_func(b_wire_requestfrom) },
    { be_const_key(init, -1), be_const_func(b_wire_init) },
    { be_const_key(write_bytes, -1), be_const_closure(write_bytes_closure) },
    { be_const_key(_begin_transmission, -1), be_const_func(b_wire_begintransmission) },
    { be_const_key(scan, -1), be_const_func(b_wire_scan) },
    { be_const_key(_write, 1), be_const_func(b_wire_write) },
    { be_const_key(write, -1), be_const_func(b_wire_validwrite) },
    { be_const_key(_available, -1), be_const_func(b_wire_available) },
    { be_const_key(detect, -1), be_const_func(b_wire_detect) },
    { be_const_key(read_bytes, -1), be_const_closure(read_bytes_closure) },
};

static be_define_const_map(
    be_class_tasmota_wire_map,
    14
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_tasmota_wire,
    1,
    NULL,
    Wire
);
