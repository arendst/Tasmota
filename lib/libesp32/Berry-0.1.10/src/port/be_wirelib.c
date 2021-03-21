/********************************************************************
 * Tasmota lib
 * 
 * To use: `import wire`
 * 
 * 2 wire communication - I2C
 *******************************************************************/
#include "be_object.h"

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

// #if !BE_USE_PRECOMPILED_OBJECT
#if 1           // TODO we will do pre-compiled later
void be_load_wirelib(bvm *vm)
{
    static const bnfuncinfo members[] = {
        { "_bus", NULL },               // bus number
        { "init", b_wire_init },
        { "_begin_transmission", b_wire_begintransmission },
        { "_end_transmission", b_wire_endtransmission },
        { "_request_from", b_wire_requestfrom },
        { "_available", b_wire_available },
        { "_write", b_wire_write },
        { "_read", b_wire_read },
        { "scan", b_wire_scan },
        { "write", b_wire_validwrite },
        { "read", b_wire_validread },
        { "detect", b_wire_detect },
        
        { NULL, NULL }
    };
    be_regclass(vm, "Wire_ntv", members);
}
#else
/* @const_object_info_begin
module tasmota (scope: global, depend: 1) {
    get_free_heap, func(l_getFreeHeap)
}
@const_object_info_end */
#include "../generate/be_fixed_tasmota.h"
#endif
