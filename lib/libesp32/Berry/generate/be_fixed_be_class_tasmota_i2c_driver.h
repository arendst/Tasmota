#include "be_constobj.h"

static be_define_const_map_slots(be_class_tasmota_i2c_driver_map) {
    { be_const_key(wire, -1), be_const_int(0) },
    { be_const_key(addr, -1), be_const_int(1) },
    { be_const_key(name, 1), be_const_int(2) },
    { be_const_key(add_cmd, -1), be_const_closure(init_closure) },
};

static be_define_const_map(
    be_class_tasmota_i2c_driver_map,
    4
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_tasmota_i2c_driver,
    3,
    (bclass *)&be_class_tasmota_driver,
    I2C_Driver
);
