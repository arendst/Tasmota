/********************************************************************
 * Tasmota lib
 * 
 * To use: `import power`
 * 
 * read power values
 *******************************************************************/
#include "be_constobj.h"
#include "be_mapping.h"

// Tasmota specific

extern int gp_member(bvm *vm);
extern int gp_pin_mode(bvm *vm);
extern int gp_digital_write(bvm *vm);
extern int gp_digital_read(bvm *vm);
extern int gp_dac_voltage(bvm *vm);
extern int gp_counter_read(bvm *vm);
extern int gp_counter_set(bvm *vm);
extern int gp_counter_add(bvm *vm);

extern int gp_pin_used(bvm *vm);
extern int gp_pin(bvm *vm);

extern int gp_get_pin(int32_t pin);               BE_FUNC_CTYPE_DECLARE(gp_get_pin, "i", "i");
extern int gp_get_pin_index(int32_t pin);               BE_FUNC_CTYPE_DECLARE(gp_get_pin_index, "i", "i");

// esp_err_tledc_set_duty_and_update(ledc_mode_tspeed_mode, ledc_channel_tchannel, uint32_t duty, uint32_t hpoint)
extern void gp_set_duty(int32_t pin, int32_t duty, int32_t hpoint);   BE_FUNC_CTYPE_DECLARE(gp_set_duty, "", "ii[i]");
extern void gp_set_frequency(int32_t pin, int32_t frequency);   BE_FUNC_CTYPE_DECLARE(gp_set_frequency, "", "ii");

extern int gp_get_duty(int32_t pin);               BE_FUNC_CTYPE_DECLARE(gp_get_duty, "i", "i");
extern int gp_get_duty_resolution(int32_t pin);    BE_FUNC_CTYPE_DECLARE(gp_get_duty_resolution, "i", "i");

/* @const_object_info_begin
module gpio (scope: global) {
    member, func(gp_member)

    pin_mode, func(gp_pin_mode)
    get_pin_type, ctype_func(gp_get_pin)
    get_pin_type_index, ctype_func(gp_get_pin_index)
    digital_write, func(gp_digital_write)
    digital_read, func(gp_digital_read)
    dac_voltage, func(gp_dac_voltage)
    counter_read, func(gp_counter_read)
    counter_set, func(gp_counter_set)
    counter_add, func(gp_counter_add)

    pin_used, func(gp_pin_used)
    pin, func(gp_pin)

    set_pwm, ctype_func(gp_set_duty)
    set_pwm_freq, ctype_func(gp_set_frequency)
    read_pwm, ctype_func(gp_get_duty)
    read_pwm_resolution, ctype_func(gp_get_duty_resolution)
}
@const_object_info_end */
#include "be_fixed_gpio.h"
