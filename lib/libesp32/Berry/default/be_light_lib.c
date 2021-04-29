/********************************************************************
 * Tasmota lib
 * 
 * To use: `import tasmota`
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LIGHT
extern int l_getlight(bvm *vm);
extern int l_setlight(bvm *vm);

extern int l_gamma8(bvm *vm);
extern int l_gamma10(bvm *vm);
extern int l_rev_gamma10(bvm *vm);

#if !BE_USE_PRECOMPILED_OBJECT

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
module light (scope: global) {
    get, func(l_getlight)
    set, func(l_setlight)

    gamma8, func(l_gamma8)
    gamma10, func(l_gamma10)
    reverse_gamma10, func(l_rev_gamma10)
}
@const_object_info_end */
#include "../generate/be_fixed_light.h"
#endif

#endif // USE_LIGHT