/********************************************************************
 * Tasmota lib
 * 
 * To use: `import wire`
 * 
 * 2 wire communication - I2C
 *******************************************************************/
#include "be_object.h"

extern int b_wire_begintransmission(bvm *vm);
extern int b_wire_endtransmission(bvm *vm);
extern int b_wire_requestfrom(bvm *vm);
extern int b_wire_available(bvm *vm);
extern int b_wire_write(bvm *vm);
extern int b_wire_read(bvm *vm);

extern int b_wire_scan(bvm *vm);

extern int b_wire_validwrite(bvm *vm);
extern int b_wire_validread(bvm *vm);

// #if !BE_USE_PRECOMPILED_OBJECT
#if 1           // TODO we will do pre-compiled later
be_native_module_attr_table(wire) {
    be_native_module_function("_begintransmission", b_wire_begintransmission),
    be_native_module_function("_endtransmission", b_wire_endtransmission),
    be_native_module_function("_requestfrom", b_wire_requestfrom),
    be_native_module_function("_available", b_wire_available),
    be_native_module_function("_write", b_wire_write),
    be_native_module_function("_read", b_wire_read),
    be_native_module_function("scan", b_wire_scan),
    be_native_module_function("write", b_wire_validwrite),
    be_native_module_function("read", b_wire_validread),
};

be_define_native_module(wire, NULL);
#else
/* @const_object_info_begin
module tasmota (scope: global, depend: 1) {
    getfreeheap, func(l_getFreeHeap)
}
@const_object_info_end */
#include "../generate/be_fixed_tasmota.h"
#endif
