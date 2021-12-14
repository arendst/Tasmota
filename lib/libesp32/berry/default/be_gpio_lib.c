/********************************************************************
 * Tasmota lib
 * 
 * To use: `import power`
 * 
 * read power values
 *******************************************************************/
#include "be_constobj.h"

// Tasmota specific

extern int gp_member(bvm *vm);
extern int gp_pin_mode(bvm *vm);
extern int gp_digital_write(bvm *vm);
extern int gp_digital_read(bvm *vm);
extern int gp_dac_voltage(bvm *vm);

extern int gp_pin_used(bvm *vm);
extern int gp_pin(bvm *vm);

/* @const_object_info_begin
module gpio (scope: global) {
    member, func(gp_member)

    pin_mode, func(gp_pin_mode)
    digital_write, func(gp_digital_write)
    digital_read, func(gp_digital_read)
    dac_voltage, func(gp_dac_voltage)

    pin_used, func(gp_pin_used)
    pin, func(gp_pin)
}
@const_object_info_end */
#include "../generate/be_fixed_gpio.h"
