#include "be_constobj.h"

static be_define_const_map_slots(be_tasmota_driver_map) {
    { be_const_key(get_tasmota, -1), be_const_func(d_getTasmotaGlob) },
    { be_const_key(web_sensor, -1), be_const_int(0) },
    { be_const_key(json_append, 0), be_const_int(1) },
    { be_const_key(every_second, 4), be_const_int(2) },
    { be_const_key(every_100ms, 7), be_const_int(3) },
    { be_const_key(web_add_main_button, -1), be_const_int(4) },
    { be_const_key(web_add_button, -1), be_const_int(5) },
    { be_const_key(save_before_restart, -1), be_const_int(6) },
    { be_const_key(button_pressed, 1), be_const_int(7) },
};

static be_define_const_map(
    be_tasmota_driver_map,
    9
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_tasmota_driver,
    8,
    NULL,
    Driver
);
