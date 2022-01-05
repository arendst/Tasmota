/********************************************************************
 * Tasmota lib
 * 
 * To use: `import wire`
 * 
 * 2 wire communication - I2C
 *******************************************************************/
#include "be_constobj.h"

#include "esp32-hal.h"

extern int b_serial_init(bvm *vm);
extern int b_serial_deinit(bvm *vm);

extern int b_serial_write(bvm *vm);
extern int b_serial_read(bvm *vm);
extern int b_serial_available(bvm *vm);
extern int b_serial_flush(bvm *vm);

#include "be_fixed_be_class_tasmota_serial.h"

void be_load_serial_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_tasmota_serial);
    be_setglobal(vm, "serial");
    be_pop(vm, 1);
}

/* @const_object_info_begin

class be_class_tasmota_serial (scope: global, name: serial) {
    .p, var

    SERIAL_5N1, int(SERIAL_5N1)
    SERIAL_6N1, int(SERIAL_6N1)
    SERIAL_7N1, int(SERIAL_7N1)
    SERIAL_8N1, int(SERIAL_8N1)
    SERIAL_5N2, int(SERIAL_5N2)
    SERIAL_6N2, int(SERIAL_6N2)
    SERIAL_7N2, int(SERIAL_7N2)
    SERIAL_8N2, int(SERIAL_8N2)
    SERIAL_5E1, int(SERIAL_5E1)
    SERIAL_6E1, int(SERIAL_6E1)
    SERIAL_7E1, int(SERIAL_7E1)
    SERIAL_8E1, int(SERIAL_8E1)
    SERIAL_5E2, int(SERIAL_5E2)
    SERIAL_6E2, int(SERIAL_6E2)
    SERIAL_7E2, int(SERIAL_7E2)
    SERIAL_8E2, int(SERIAL_8E2)
    SERIAL_5O1, int(SERIAL_5O1)
    SERIAL_6O1, int(SERIAL_6O1)
    SERIAL_7O1, int(SERIAL_7O1)
    SERIAL_8O1, int(SERIAL_8O1)
    SERIAL_5O2, int(SERIAL_5O2)
    SERIAL_6O2, int(SERIAL_6O2)
    SERIAL_7O2, int(SERIAL_7O2)
    SERIAL_8O2, int(SERIAL_8O2)

    init, func(b_serial_init)
    deinit, func(b_serial_deinit)

    write, func(b_serial_write)
    read, func(b_serial_read)
    available, func(b_serial_available)
    flush, func(b_serial_flush)
}
@const_object_info_end */
