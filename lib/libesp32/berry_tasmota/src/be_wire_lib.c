/********************************************************************
 * Tasmota lib
 * 
 * To use: `import wire`
 * 
 * 2 wire communication - I2C
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_I2C

extern int b_wire_init(bvm *vm);

extern int b_wire_begintransmission(bvm *vm);
extern int b_wire_endtransmission(bvm *vm);
extern int b_wire_requestfrom(bvm *vm);
extern int b_wire_available(bvm *vm);
extern int b_wire_write(bvm *vm);
extern int b_wire_read(bvm *vm);

extern int b_wire_scan(bvm *vm);

extern int b_wire_validwrite(bvm *vm);
extern int b_wire_validread(bvm *vm);
extern int b_wire_detect(bvm *vm);
extern int b_wire_enabled(bvm *vm);

#include "solidify/solidified_Wire.h"
#include "be_fixed_be_class_Wire.h"

/* @const_object_info_begin

class be_class_Wire (scope: global, name: Wire) {
    bus, var

    init, func(b_wire_init)
    _begin_transmission, func(b_wire_begintransmission)
    _end_transmission, func(b_wire_endtransmission)
    _request_from, func(b_wire_requestfrom)
    _available, func(b_wire_available)
    _write, func(b_wire_write)
    _read, func(b_wire_read)
    scan, func(b_wire_scan)
    write, func(b_wire_validwrite)
    read, func(b_wire_validread)
    detect, func(b_wire_detect)
    enabled, func(b_wire_enabled)

    read_bytes, closure(class_Wire_read_bytes_closure)
    write_bytes, closure(class_Wire_write_bytes_closure)
}
@const_object_info_end */

#endif // USE_I2C