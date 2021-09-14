#include "be_constobj.h"

static be_define_const_map_slots(be_class_I2C_Driver_map) {
    { be_const_key(read32, -1), be_const_closure(read32_closure) },
    { be_const_key(write8, 6), be_const_closure(write8_closure) },
    { be_const_key(name, -1), be_const_var(0) },
    { be_const_key(addr, 10), be_const_var(1) },
    { be_const_key(read12, -1), be_const_closure(read12_closure) },
    { be_const_key(write_bit, 8), be_const_closure(write_bit_closure) },
    { be_const_key(read13, -1), be_const_closure(read13_closure) },
    { be_const_key(read24, -1), be_const_closure(read24_closure) },
    { be_const_key(wire, -1), be_const_var(2) },
    { be_const_key(init, -1), be_const_closure(init_closure) },
    { be_const_key(read8, -1), be_const_closure(read8_closure) },
};

static be_define_const_map(
    be_class_I2C_Driver_map,
    11
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_I2C_Driver,
    3,
    (bclass *)&be_class_Driver,
    I2C_Driver
);
