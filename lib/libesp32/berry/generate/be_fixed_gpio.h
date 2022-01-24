#include "be_constobj.h"

static be_define_const_map_slots(m_libgpio_map) {
    { be_const_key(digital_read, -1), be_const_func(gp_digital_read) },
    { be_const_key(digital_write, -1), be_const_func(gp_digital_write) },
    { be_const_key(dac_voltage, -1), be_const_func(gp_dac_voltage) },
    { be_const_key(member, 1), be_const_func(gp_member) },
    { be_const_key(set_pwm, 5), be_const_ctype_func(gp_set_duty) },
    { be_const_key(pin, 7), be_const_func(gp_pin) },
    { be_const_key(pin_mode, 2), be_const_func(gp_pin_mode) },
    { be_const_key(pin_used, -1), be_const_func(gp_pin_used) },
};

static be_define_const_map(
    m_libgpio_map,
    8
);

static be_define_const_module(
    m_libgpio,
    "gpio"
);

BE_EXPORT_VARIABLE be_define_const_native_module(gpio);
