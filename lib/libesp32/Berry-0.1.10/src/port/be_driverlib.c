/********************************************************************
 * Tasmota lib
 * 
 * To use: `d = Driver()`
 * 
 *******************************************************************/
#include "be_object.h"

// #if !BE_USE_PRECOMPILED_OBJECT
#if 1           // TODO we will do pre-compiled later
void be_load_driverlib(bvm *vm)
{
    static const bnfuncinfo members[] = {
        { "every_second", NULL },
        { "every_100ms", NULL },
        { "web_add_button", NULL },
        { "web_add_main_button", NULL },
        { "save_before_restart", NULL },
        { "web_sensor", NULL },
        { "json_append", NULL },
        { "button_pressed", NULL },
        
        { NULL, NULL }
    };
    be_regclass(vm, "Driver", members);
}
#else
/* @const_object_info_begin
module tasmota (scope: global, depend: 1) {
    get_free_heap, func(l_getFreeHeap)
}
@const_object_info_end */
#include "../generate/be_fixed_tasmota.h"
#endif
