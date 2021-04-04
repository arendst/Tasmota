/********************************************************************
 * Tasmota lib
 * 
 * To use: `import tasmota`
 *******************************************************************/
#include "be_object.h"
#include "be_string.h"
#include "be_gc.h"


extern int l_getlight(bvm *vm);
extern int l_setlight(bvm *vm);

extern int l_gamma8(bvm *vm);
extern int l_gamma10(bvm *vm);
extern int l_rev_gamma10(bvm *vm);

// #if !BE_USE_PRECOMPILED_OBJECT
#if 1           // TODO we will do pre-compiled later

be_native_module_attr_table(light) {

    be_native_module_function("get", l_getlight),
    be_native_module_function("set", l_setlight),

    be_native_module_function("gamma8", l_gamma8),
    be_native_module_function("gamma10", l_gamma10),
    be_native_module_function("reverse_gamma10", l_rev_gamma10),

};

be_define_native_module(light, NULL);

#else
/* @const_object_info_begin
module tasmota (scope: global, depend: 1) {
    get_free_heap, func(l_getFreeHeap)
}
@const_object_info_end */
#include "../generate/be_fixed_tasmota.h"
#endif
