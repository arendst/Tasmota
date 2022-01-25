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

extern int gp_pin_used(bvm *vm);
extern int gp_pin(bvm *vm);

// esp_err_tledc_set_duty_and_update(ledc_mode_tspeed_mode, ledc_channel_tchannel, uint32_t duty, uint32_t hpoint)
extern void gp_set_duty(int32_t pin, int32_t duty, int32_t hpoint);   BE_FUNC_CTYPE_DECLARE(gp_set_duty, "", "ii[i]");


/* @const_object_info_begin
module gpio (scope: global) {
    member, func(gp_member)

    pin_mode, func(gp_pin_mode)
    digital_write, func(gp_digital_write)
    digital_read, func(gp_digital_read)
    dac_voltage, func(gp_dac_voltage)

    pin_used, func(gp_pin_used)
    pin, func(gp_pin)

    set_pwm, ctype_func(gp_set_duty)
}
@const_object_info_end */
#include "be_fixed_gpio.h"
