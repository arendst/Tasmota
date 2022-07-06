#include "be_constobj.h"

static be_define_const_map_slots(m_libULP_map) {
    { be_const_key(sleep, -1), be_const_ctype_func(be_ULP_sleep) },
    { be_const_key(get_mem, 6), be_const_ctype_func(be_ULP_get_mem) },
    { be_const_key(run, -1), be_const_ctype_func(be_ULP_run) },
    { be_const_key(load, -1), be_const_ctype_func(be_ULP_load) },
    { be_const_key(adc_config, -1), be_const_ctype_func(be_ULP_adc_config) },
    { be_const_key(set_mem, -1), be_const_ctype_func(be_ULP_set_mem) },
    { be_const_key(gpio_init, 3), be_const_ctype_func(be_ULP_gpio_init) },
    { be_const_key(wake_period, -1), be_const_ctype_func(be_ULP_wake_up_period) },
};

static be_define_const_map(
    m_libULP_map,
    8
);

static be_define_const_module(
    m_libULP,
    "ULP"
);

BE_EXPORT_VARIABLE be_define_const_native_module(ULP);
