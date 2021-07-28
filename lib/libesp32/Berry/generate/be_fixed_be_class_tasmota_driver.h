#include "be_constobj.h"

static be_define_const_map_slots(be_class_tasmota_driver_map) {
    { be_const_key(button_pressed, 6), be_const_index(0) },
    { be_const_key(web_add_management_button, -1), be_const_index(1) },
    { be_const_key(web_add_button, 13), be_const_index(2) },
    { be_const_key(web_add_config_button, -1), be_const_index(3) },
    { be_const_key(json_append, -1), be_const_index(4) },
    { be_const_key(display, 3), be_const_index(5) },
    { be_const_key(web_add_console_button, 14), be_const_index(6) },
    { be_const_key(every_100ms, -1), be_const_index(7) },
    { be_const_key(every_50ms, -1), be_const_index(8) },
    { be_const_key(every_second, -1), be_const_index(9) },
    { be_const_key(save_before_restart, -1), be_const_index(10) },
    { be_const_key(get_tasmota, -1), be_const_closure(get_tasmota_closure) },
    { be_const_key(web_sensor, 7), be_const_index(11) },
    { be_const_key(web_add_handler, 1), be_const_index(12) },
    { be_const_key(web_add_main_button, -1), be_const_index(13) },
    { be_const_key(add_cmd, -1), be_const_closure(add_cmd_closure) },
};

static be_define_const_map(
    be_class_tasmota_driver_map,
    16
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_tasmota_driver,
    14,
    NULL,
    Driver
);
